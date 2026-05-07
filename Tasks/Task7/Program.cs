using System;
using System.Collections.Generic;

namespace Task7_2200487
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Single-Machine Scheduler ===");
            Console.WriteLine("Minimize total penalty for tardy jobs.\n");

            Console.Write("Enter number of jobs: ");
            if (!int.TryParse(Console.ReadLine(), out int n) || n <= 0)
            {
                Console.WriteLine("Invalid number of jobs.");
                return;
            }

            var jobs = new Job[n];
            for (int i = 0; i < n; i++)
            {
                Console.Write($"Job {i + 1} — processing time t, penalty p, deadline d (space-separated): ");
                var parts = Console.ReadLine()!.Split(' ', StringSplitOptions.RemoveEmptyEntries);
                jobs[i] = new Job
                {
                    Id = i + 1,
                    ProcessingTime = int.Parse(parts[0]),
                    Penalty = int.Parse(parts[1]),
                    Deadline = int.Parse(parts[2])
                };
            }

            var algorithms = new List<ISchedulerAlgorithm>
            {
                new BruteForceScheduler(),
                new DivideAndConquerScheduler(),
                new DynamicProgrammingScheduler()
            };

            foreach (var algo in algorithms)
            {
                Console.WriteLine($"\nRunning: {algo.Name}...");
                var result = algo.Solve(jobs);

                Console.WriteLine($"──────────────────────────────────────");
                Console.WriteLine($"  Total penalty of all jobs : {result.TotalPossiblePenalty}");
                Console.WriteLine($"  Maximum penalty saved     : {result.MaxSavedPenalty}");
                Console.WriteLine($"  Minimum total penalty     : {result.MinTotalPenalty}");
                Console.WriteLine($"──────────────────────────────────────");

                if (result.OnTimeJobIds != null && result.TardyJobIds != null)
                {
                    Console.WriteLine($"  Jobs Done (On-Time)       : [{(result.OnTimeJobIds.Count > 0 ? string.Join(" -> ", result.OnTimeJobIds) : "None")}]");
                    Console.WriteLine($"  Jobs Not Done (Tardy)     : [{(result.TardyJobIds.Count > 0 ? string.Join(", ", result.TardyJobIds) : "None")}]");
                    Console.WriteLine($"──────────────────────────────────────");
                }
            }
        }
    }
}
