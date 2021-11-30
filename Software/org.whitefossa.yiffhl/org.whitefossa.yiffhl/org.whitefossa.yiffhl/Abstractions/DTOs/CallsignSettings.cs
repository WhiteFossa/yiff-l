namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Settings, related to callsign
    /// </summary>
    public class CallsignSettings
    {
        /// <summary>
        /// Current callsign
        /// </summary>
        public Callsign Callsing { get; set; }

        /// <summary>
        /// Transmission speed
        /// </summary>
        public bool IsFast { get; set; }
    }
}
