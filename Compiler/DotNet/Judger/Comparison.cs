using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Judger
{
    class Comparison
    { 

        private static string Soft(string str)
        {
            StringBuilder stringBuilder = new StringBuilder();
            foreach (var i in str)
            {
                if (i != ' ' && i != '\t' && i != '\r' && i != '\n')
                {
                    stringBuilder.Append(i);
                }
            }
            return stringBuilder.ToString();
        }
        private static string Format(string str)
        { 
            StringBuilder stringBuilder = new StringBuilder();
            foreach (var i in str)
            {
                if (i != '\r')
                {
                    stringBuilder.Append(i);
                }
            }
            return stringBuilder.ToString().TrimEnd(" \n".ToCharArray());
        }


        public static JudgeResultType Compare(string standardOutput, string testOutput)
        {
            if (Format(standardOutput) == Format(testOutput))
            {
                return JudgeResultType.Accept;
            }
            if (Soft(standardOutput) == Soft(testOutput))
            {
                return JudgeResultType.PresentationError;
            }
            return JudgeResultType.WrongAnswer; 
        } 
    }
}
