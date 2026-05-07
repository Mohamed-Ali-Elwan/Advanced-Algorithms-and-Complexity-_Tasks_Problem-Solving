using System;
using System.Collections.Generic;

namespace Task7_2200487
{
    internal class DivideAndConquerScheduler : ISchedulerAlgorithm
    {
        public string Name => "Divide & Conquer Scheduler";

        public SchedulerResult Solve(Job[] jobs)
        {
            var sortedJobs = (Job[])jobs.Clone();
            SortHelper.MergeSort(sortedJobs, (a, b) => a.Deadline.CompareTo(b.Deadline));

            var frontier = SolveRecursive(sortedJobs, 0, sortedJobs.Length - 1);

            int totalPenalty = 0;
            foreach (var job in sortedJobs)
                totalPenalty += job.Penalty;

            int maxSaved = 0;
            ParetoState bestState = default;
            foreach (var s in frontier)
            {
                if (s.SavedPenalty > maxSaved)
                {
                    maxSaved = s.SavedPenalty;
                    bestState = s;
                }
            }

            int minPenalty = totalPenalty - maxSaved;

            var onTimeJobs = new List<int>();
            var tardyJobs = new List<int>();
            
            if (bestState.JobIds != null)
            {
                for (int i = 0; i < bestState.JobIds.Count; i++)
                {
                    onTimeJobs.Add(bestState.JobIds[i]);
                }
            }

            for (int i = 0; i < sortedJobs.Length; i++)
            {
                bool isFound = false;
                for (int j = 0; j < onTimeJobs.Count; j++)
                {
                    if (onTimeJobs[j] == sortedJobs[i].Id)
                    {
                        isFound = true;
                        break;
                    }
                }
                
                if (!isFound)
                {
                    tardyJobs.Add(sortedJobs[i].Id);
                }
            }

            return new SchedulerResult
            {
                TotalPossiblePenalty = totalPenalty,
                MaxSavedPenalty = maxSaved,
                MinTotalPenalty = minPenalty,
                OnTimeJobIds = onTimeJobs,
                TardyJobIds = tardyJobs,
                FinalParetoFrontier = frontier
            };
        }

        private List<ParetoState> SolveRecursive(Job[] jobs, int lo, int hi)
        {

            if (lo == hi)
            {
                var states = new List<ParetoState>();

 
                states.Add(new ParetoState(0, 0, 0, new List<int>()));

   
                if (jobs[lo].ProcessingTime <= jobs[lo].Deadline)
                {
                    states.Add(new ParetoState(
                        jobs[lo].ProcessingTime,
                        jobs[lo].Penalty,
                        jobs[lo].Deadline,
                        new List<int> { jobs[lo].Id }));
                }

                return states;
            }

            int mid = (lo + hi) / 2;
            var leftStates = SolveRecursive(jobs, lo, mid);
            var rightStates = SolveRecursive(jobs, mid + 1, hi);

            var merged = new List<ParetoState>();

            foreach (var L in leftStates)
            {
                foreach (var R in rightStates)
                {
                    int tauNew = L.Tau + R.Tau;
                    int profitNew = L.SavedPenalty + R.SavedPenalty;

                    if (R.Tau > 0 && tauNew > R.MaxDeadline)
                        continue;

                    int maxDeadline = Math.Max(L.MaxDeadline, R.MaxDeadline);

                    var newJobIds = new List<int>(L.JobIds.Count + R.JobIds.Count);
                    
                    for (int i = 0; i < L.JobIds.Count; i++)
                        newJobIds.Add(L.JobIds[i]);
                        
                    for (int i = 0; i < R.JobIds.Count; i++)
                        newJobIds.Add(R.JobIds[i]);

                    merged.Add(new ParetoState(tauNew, profitNew, maxDeadline, newJobIds));
                }
            }

            SortHelper.MergeSort(merged, (a, b) =>
            {
                int cmp = a.Tau.CompareTo(b.Tau);
                return cmp != 0 ? cmp : b.SavedPenalty.CompareTo(a.SavedPenalty);
            });

            var pruned = new List<ParetoState>();
            int bestProfit = -1;

            foreach (var s in merged)
            {

                if (s.SavedPenalty > bestProfit)
                {
                    pruned.Add(s);
                    bestProfit = s.SavedPenalty;
                }
            }

            return pruned;
        }
    }
}
