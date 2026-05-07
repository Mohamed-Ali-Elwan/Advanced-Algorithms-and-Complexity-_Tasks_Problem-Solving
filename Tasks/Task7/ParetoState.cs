using System.Collections.Generic;

namespace Task7_2200487
{

    internal struct ParetoState
    {
        public int Tau { get; set; }          
        public int SavedPenalty { get; set; } 
        public int MaxDeadline { get; set; }
        public List<int> JobIds { get; set; }

        public ParetoState(int tau, int savedPenalty, int maxDeadline, List<int> jobIds)
        {
            Tau = tau;
            SavedPenalty = savedPenalty;
            MaxDeadline = maxDeadline;
            JobIds = jobIds;
        }

        public override string ToString() => $"(τ={Tau}, P={SavedPenalty}, dMax={MaxDeadline})";
    }
}
