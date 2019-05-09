using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace Judger
{

    class CJudger
    {
        private long timeLimit;
        private long memoryLimit;
        private Topic topic;

        void Compile(string compileArgument)
        {
            var gccPath = @"MinGW\bin\gcc.exe";
            var codePath = @"Code\C\a.c";
            var outputPath = @"Code\C\a.run";
            if (!File.Exists(gccPath))
            {
                throw new JudgeInnerException(string.Format("{0} not exist", gccPath));
            }
            bool needRestart = true;
            while (needRestart)
            {
                needRestart = false;
                using (Process myProcess = new Process())
                {
                    myProcess.StartInfo.UseShellExecute = false;
                    myProcess.StartInfo.FileName = gccPath;
                    myProcess.StartInfo.CreateNoWindow = true;
                    myProcess.StartInfo.RedirectStandardError = true;
                    myProcess.StartInfo.Arguments = string.Format("{0} {1} -o {2} ", codePath, compileArgument, outputPath);
                    myProcess.Start();
                    if (!myProcess.WaitForExit(10 * 1000))
                    {
                        myProcess.Kill();
                        myProcess.WaitForExit();
                        throw new JudgeResultException(JudgeResultType.CompileTimeLimitExceed, 0, 0);
                    }
                    var errorInfo = myProcess.StandardError.ReadToEnd();
                    if (errorInfo.Length > 0)
                    {
                        System.IO.StreamWriter file = new StreamWriter(@"Code\CompilerErrorLog", true);
                        file.WriteLine(errorInfo);
                        file.Close();
                        if (errorInfo.Contains("ld.exe: cannot open output file"))
                        {// file not ready, restart, gcc bug
                            needRestart = true;
                        }
                        else
                        {
                            throw new JudgeResultException(JudgeResultType.CompileError, 0, 0);
                        }
                    }
                    else if (myProcess.ExitCode != 0)
                    {
                        throw new JudgeInnerException("error code: " + myProcess.ExitCode);
                    }
                }
            }
        }

        public JudgeResult Run(long time, long memory, Topic myTopic)
        {
            Compile(" -pipe -static -lm -s -std=c99 -O2");
            timeLimit = time;
            memoryLimit = memory;
            topic = myTopic;
            return Execute();
        }

        public JudgeResult Execute()
        {
            string programPath = @"Code\C\a.run";
            using (Process myProcess = new Process())
            {
                StringBuilder output = new StringBuilder();
                myProcess.StartInfo.UseShellExecute = false;
                myProcess.StartInfo.FileName = programPath;
                myProcess.StartInfo.CreateNoWindow = true;
                myProcess.StartInfo.UseShellExecute = false;
                myProcess.StartInfo.RedirectStandardOutput = true;
                myProcess.StartInfo.RedirectStandardInput = true;

                myProcess.OutputDataReceived += new DataReceivedEventHandler((s, e) =>
                {
                    if (!string.IsNullOrEmpty(e.Data))
                    {
                        output.Append(e.Data);
                        output.Append('\n');
                    }
                });

                double timeMax = 0;
                double memoryMax = 0;
                foreach (var sample in topic.Sample)
                {
                    output.Clear();

                    double time = 0;
                    double memory = 0;
                    bool tag = false;
                    myProcess.Start();

                    myProcess.StandardInput.Write(sample.Input);
                    myProcess.StandardInput.Close();

                    int memorySetCount = 0;
                    do
                    {
                        if (!tag)
                        {
                            myProcess.BeginOutputReadLine();
                            tag = true;
                        } 

                        try
                        {
                            time = myProcess.TotalProcessorTime.TotalMilliseconds;
                        }
                        catch (InvalidOperationException)
                        {
                            // process exited exception, do nothing
                        }
                        if (memorySetCount == 0 || myProcess.HasExited)
                        {
                            memorySetCount = 5;
                            try
                            {
                                memory = myProcess.PrivateMemorySize64 / 1024.0 / 1024;
                            }
                            catch (InvalidOperationException)
                            {
                                // exception, process may ha exited, do nothing
                            }
                        }
                        memorySetCount -= 1;

                        if (time > timeLimit)
                        {
                            myProcess.Kill();
                            throw new JudgeResultException(JudgeResultType.TimeLimiExceed, time, memory);
                        }
                        if (memory > memoryLimit)
                        {
                            myProcess.Kill();
                            throw new JudgeResultException(JudgeResultType.MemoryLimitExceed, time, memory);
                        }
                        System.Threading.Thread.Sleep(20);
                    } while ((!myProcess.HasExited));
                    myProcess.WaitForExit();
                    myProcess.CancelOutputRead();
                    if (myProcess.ExitCode != 0)
                    {
                        throw new JudgeResultException(JudgeResultType.RuntimeError, time, memory);
                    }

                    var result = Comparison.Compare(sample.Output, output.ToString());
                    if (result != JudgeResultType.Accept)
                    {
                        throw new JudgeResultException(result, time, memory);
                    }


                    timeMax = Math.Max(timeMax, time);
                    memoryMax = Math.Max(memoryMax, memory);
                }
                return new JudgeResult(JudgeResultType.Accept, timeMax, memoryMax);
            }
        }
    }
}
