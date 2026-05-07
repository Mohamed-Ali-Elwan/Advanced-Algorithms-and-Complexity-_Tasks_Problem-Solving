using System;
using System.Collections.Generic;

namespace Task7_2200487
{

    internal class DynamicProgrammingScheduler : ISchedulerAlgorithm
    {
        public string Name => "Dynamic Programming Scheduler";

        private const int NEG_INF = int.MinValue; 

        public SchedulerResult Solve(Job[] jobs)
        {
            int n = jobs.Length;

            var sortedJobs = new Job[n];
            for (int i = 0; i < n; i++)
                sortedJobs[i] = jobs[i];

            SortHelper.MergeSort(sortedJobs, (a, b) => a.Deadline.CompareTo(b.Deadline));

            int totalPenalty = 0;
            for (int i = 0; i < n; i++)
                totalPenalty += sortedJobs[i].Penalty;

            int columns = sortedJobs[n - 1].Deadline;

            //Build the DP table
            int[,] V = BuildDPTable(sortedJobs, n, columns);

            //Extract the optimal value
            int maxSaved = 0;
            int bestTau = 0;
            ExtractOptimal(V, n, columns, out maxSaved, out bestTau);

            int minPenalty = totalPenalty - maxSaved;

            //Backtrack to find on-time and tardy jobs
            var onTimeJobs = new List<int>();
            var tardyJobs = new List<int>();
            Backtrack(V, sortedJobs, n, bestTau, onTimeJobs, tardyJobs);

            return new SchedulerResult
            {
                TotalPossiblePenalty = totalPenalty,
                MaxSavedPenalty = maxSaved,
                MinTotalPenalty = minPenalty,
                OnTimeJobIds = onTimeJobs,
                TardyJobIds = tardyJobs,
                FinalParetoFrontier = null
            };
        }


        private int[,] BuildDPTable(Job[] sortedJobs, int n, int columns)
        {
            int[,] V = new int[n + 1, columns + 1];

            V[0, 0] = 0;
            for (int tau = 1; tau <= columns; tau++)
                V[0, tau] = NEG_INF;


            for (int i = 1; i <= n; i++)
            {
                int ti = sortedJobs[i - 1].ProcessingTime;
                int pi = sortedJobs[i - 1].Penalty;
                int di = sortedJobs[i - 1].Deadline;

                for (int tau = 0; tau <= columns; tau++)
                {
                    V[i, tau] = V[i - 1, tau];

                    if (tau >= ti && tau <= di)
                    {
                        int prev = V[i - 1, tau - ti];

                        if (prev != NEG_INF)
                        {
                            int includeValue = prev + pi;

                            if (includeValue > V[i, tau])
                                V[i, tau] = includeValue;
                        }
                    }
                }
            }

            return V;
        }

        private void ExtractOptimal(int[,] V, int n, int columns,
            out int maxSaved, out int bestTau)
        {
            maxSaved = 0;
            bestTau = 0;

            for (int tau = 0; tau <= columns; tau++)
            {
                if (V[n, tau] > maxSaved)
                {
                    maxSaved = V[n, tau];
                    bestTau = tau;
                }
            }
        }

        private void Backtrack(int[,] V, Job[] sortedJobs, int n, int bestTau,
            List<int> onTimeJobs, List<int> tardyJobs)
        {
            int currentTau = bestTau;

            for (int i = n; i >= 1; i--)
            {
                if (V[i, currentTau] != V[i - 1, currentTau])
                {
                    onTimeJobs.Add(sortedJobs[i - 1].Id);
                    currentTau -= sortedJobs[i - 1].ProcessingTime;
                }
                else
                {
                    tardyJobs.Add(sortedJobs[i - 1].Id);
                }
            }

            for (int i = 0, j = onTimeJobs.Count - 1; i < j; i++, j--)
            {
                int tmp = onTimeJobs[i];
                onTimeJobs[i] = onTimeJobs[j];
                onTimeJobs[j] = tmp;
            }
        }
    }
}
