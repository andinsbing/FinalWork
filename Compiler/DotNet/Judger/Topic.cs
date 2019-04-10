using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Judger
{
    public class TopicSample
    {
        public string Input { get; }
        public string Output { get; }
        public TopicSample(string input, string output)
        {
            Input = input;
            Output = output;
        }
    }

    class Topic
    {  
        private readonly string topicPath = @"Test"; 
        public List<TopicSample> Sample { get; }

        public Topic()
        {
            Sample = new List<TopicSample>();
        }

        public void Read(int index)
        {
            var path = string.Format(@"{0}\Topic{1}", topicPath, index);
         
            if(!Directory.Exists(path))
            { 
                throw new JudgeInnerException(string.Format("{0} not exist", Path.GetFullPath(path)));
            } 
            for(int i=1; ;i++)
            {
                var inputPath = string.Format(@"{0}\{1}.in", path, i);
                var outputPath = string.Format(@"{0}\{1}.out", path, i);
                if (!File.Exists(inputPath) || !File.Exists(outputPath))
                {
                    break;
                }
                Sample.Add(new TopicSample(File.ReadAllText(inputPath), File.ReadAllText(outputPath))); 
            }
            if(Sample.Count==0)
            {
                throw new JudgeInnerException("sample can not be founded");
            }
        }
    }
}
