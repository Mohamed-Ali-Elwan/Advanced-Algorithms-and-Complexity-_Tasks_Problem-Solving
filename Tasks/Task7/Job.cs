namespace Task7_2200487
{

    internal class Job
    {
        public int Id { get; set; }
        public int ProcessingTime { get; set; }  
        public int Penalty { get; set; }         
        public int Deadline { get; set; }        

        public override string ToString() => $"Job {Id} (t={ProcessingTime}, p={Penalty}, d={Deadline})";
    }
}
