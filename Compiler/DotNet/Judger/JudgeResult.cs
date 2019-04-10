using System;

namespace Judger
{
    public enum JudgeResultType
    {
        Accept,
        WrongAnswer,
        PresentationError,
        TimeLimiExceed,
        OutputLimitExceed,
        MemoryLimitExceed,
        RuntimeError,
        CompileError,
        CompileTimeLimitExceed,
        JudgerInnerError,
    }

    public class JudgeResultException : ApplicationException
    {
        public JudgeResult Result { get; }
        public JudgeResultException(JudgeResult result)
        {
            Result = result;
        }
        public JudgeResultException(JudgeResultType type, double time, double memory)
        {
            Result = new JudgeResult(type, time, memory);
        }
    }

    public class JudgeInnerException : ApplicationException
    { 
        public JudgeInnerException(string message) :base(message)
        {
          
        }
    }

    public class JudgeResult
    {
        public JudgeResultType Type { get; }
        public double Time { get; }
        public double Memory { get; }
        public JudgeResult(JudgeResultType type, double time, double memory)
        {
            Type = type;
            Time = time;
            Memory = memory;
        }
    }

}
