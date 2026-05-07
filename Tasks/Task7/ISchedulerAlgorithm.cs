using System.Collections.Generic;

namespace Task7_2200487
{
    internal class SchedulerResult
    {
        public int MinTotalPenalty { get; set; }
        public int MaxSavedPenalty { get; set; }
        public int TotalPossiblePenalty { get; set; }
        public List<int>? OnTimeJobIds { get; set; }
        public List<int>? TardyJobIds { get; set; }
        public List<ParetoState>? FinalParetoFrontier { get; set; }
    }

    internal interface ISchedulerAlgorithm
    {
        string Name { get; }
        SchedulerResult Solve(Job[] jobs);
    }
}
