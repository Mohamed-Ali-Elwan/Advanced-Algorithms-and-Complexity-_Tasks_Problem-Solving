using System;
using System.Collections.Generic;

namespace Task7_2200487
{

    internal class BruteForceScheduler : ISchedulerAlgorithm
    {
        public string Name => "Brute Force Scheduler";

        private int _globalMinPenalty;
        private int[] _bestSequence = null;

        public SchedulerResult Solve(Job[] jobs)
        {
            int n = jobs.Length;

            _globalMinPenalty = int.MaxValue;
            _bestSequence = new int[n];

            int totalPenalty = 0;
            for (int i = 0; i < n; i++)
                totalPenalty += jobs[i].Penalty;

            bool[] used = new bool[n];       
            int[] sequence = new int[n];     

            TryAllOrders(jobs, used, sequence, 0, n, 0, 0);

            int maxSaved = totalPenalty - _globalMinPenalty;

            var onTimeJobs = new List<int>();
            var tardyJobs = new List<int>();
            int elapsedTime = 0;

            for (int i = 0; i < n; i++)
            {
                int jobIndex = _bestSequence[i];
                elapsedTime += jobs[jobIndex].ProcessingTime;

                if (elapsedTime <= jobs[jobIndex].Deadline)
                {
                    onTimeJobs.Add(jobs[jobIndex].Id);
                }
                else
                {
                    tardyJobs.Add(jobs[jobIndex].Id);
                }
            }

            return new SchedulerResult
            {
                TotalPossiblePenalty = totalPenalty,
                MaxSavedPenalty = maxSaved,
                MinTotalPenalty = _globalMinPenalty,
                OnTimeJobIds = onTimeJobs,
                TardyJobIds = tardyJobs,
                FinalParetoFrontier = null
            };
        }

        private void TryAllOrders(Job[] jobs, bool[] used, int[] sequence,
            int depth, int n, int elapsedTime, int penalty)
        {
            if (depth == n)
            {
                if (penalty < _globalMinPenalty)
                {
                    _globalMinPenalty = penalty;
                    for (int i = 0; i < n; i++)
                        _bestSequence[i] = sequence[i];
                }
                return;
            }


            for (int i = 0; i < n; i++)
            {
                if (used[i])
                    continue;

                used[i] = true;
                sequence[depth] = i;

                int newElapsed = elapsedTime + jobs[i].ProcessingTime;
                int newPenalty = penalty;

                if (newElapsed > jobs[i].Deadline)
                    newPenalty += jobs[i].Penalty;

                TryAllOrders(jobs, used, sequence, depth + 1, n, newElapsed, newPenalty);

                used[i] = false;
            }
        }
    }
}
