namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Static fox status (no need to poll, just read on connect)
    /// </summary>
    public class StaticFoxStatus
    {
        /// <summary>
        /// If true, then fox is on countdown / on the run
        /// </summary>
        public bool IsFoxArmed { get; set; }
    }
}
