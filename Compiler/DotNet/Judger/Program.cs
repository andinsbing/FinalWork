using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.ComponentModel;
using System.IO;
using System.Threading;


namespace Judger
{
    enum JudgetType
    {
        CPP,
        JAVA,
        C
    } 
     
    class Program
    { 

        private static void Initialize(string[] arg, out int topicID, out int time, out int memory, out JudgetType type, out Topic topic)
        {
            time = 0;
            memory = 0;
            type = 0;
            topicID = 0;
            topic = new Topic();

            bool timeTag = false;
            bool memoryTag = false;
            bool typeTag = false;
            bool topicTag = false;

            if (arg.Length % 2 != 0)
            {
                throw new JudgeInnerException("arguments size cannot be odd");
            }
            for (int i = 0; i < arg.Length; i += 2)
            {
                switch (arg[i])
                {
                    case "-t":
                        time = int.Parse(arg[i + 1]);
                        timeTag = true;
                        break;
                    case "-m":
                        memory = int.Parse(arg[i + 1]);
                        memoryTag = true;
                        break;
                    case "-i":
                        topicID = int.Parse(arg[i + 1]);
                        topicTag = true;
                        break;
                    case "-c":
                        switch (arg[i + 1])
                        {
                            case "CPP":
                                type = JudgetType.CPP;
                                typeTag = true;
                                break;
                            case "JAVA":
                                type = JudgetType.JAVA;
                                typeTag = true;
                                break;
                            case "C":
                                type = JudgetType.C;
                                typeTag = true;
                                break;
                            default:
                                throw new JudgeInnerException($"unrecognized type:{arg[i + 1]}");
                        }
                        break;
                    default:
                        throw new JudgeInnerException("unrecognized argument:" + arg[i + 1]);
                }
            }
            if (!timeTag || !memoryTag || !typeTag || !topicTag)
            {
                throw new JudgeInnerException("initialization arguments are not enough");
            }
            topic.Read(topicID);
        }

        // -t time limit(ms)
        // -m memory limit(ms)
        // -c CPP|C|JAVA
        static int Main(string[] args)
        {
            System.Environment.CurrentDirectory = @"F:\QtProject\FinalWork\Compiler";
            JudgeResult result = null;
            try
            {
                Initialize(args, out int topicID, out int timeLimit, out int memoryLimit, out JudgetType type, out Topic topic);
                switch (type)
                {
                    case JudgetType.CPP:
                        {
                            CPPJudger judger = new CPPJudger();
                            result = judger.Run(timeLimit, memoryLimit, topic);
                            break;
                        }
                    case JudgetType.JAVA:
                        {
                            JAVAJudger judger = new JAVAJudger();
                            result = judger.Run(timeLimit, memoryLimit, topic);
                            break;
                        }
                    case JudgetType.C:
                        {
                            CJudger judger = new CJudger();
                            result  = judger.Run(timeLimit, memoryLimit, topic);
                            break;
                        }
                    default:
                        throw new Exception("unrecognized type");
                }
                Console.Write(string.Format("{0},{1},{2}", result.Type, result.Time, result.Memory));
            }
            catch (JudgeResultException e)
            {
                var message = string.Format("{0},{1},{2}", e.Result.Type, e.Result.Time, e.Result.Memory);
                var bytes = Encoding.UTF8.GetBytes(message);
                Console.Write(Encoding.UTF8.GetString(bytes));
            }
            catch (JudgeInnerException e)
            {
                var bytes = Encoding.UTF8.GetBytes(e.Message);
                Console.Write(Encoding.UTF8.GetString(bytes));
            }
            catch (Exception e)
            {
                File.AppendAllText("exception", $"--{System.DateTime.Now}\n--{e.StackTrace}\n--{e.Data}\n--{e.Message}--{e.GetType()}--{e}\n");
                var bytes = Encoding.UTF8.GetBytes(e.Message);
                Console.Write(Encoding.UTF8.GetString(bytes));
            }
            Console.ReadKey();
            return 0;
            }
    }
}
