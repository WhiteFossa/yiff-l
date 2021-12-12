namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Profile settings
    /// </summary>
    public class ProfileSettings
    {
        /// <summary>
        /// Profile frequency settings
        /// </summary>
        public FrequencySettings FrequencySettings { get; set; } = new FrequencySettings();

        /// <summary>
        /// Callsing settings
        /// </summary>
        public CallsignSettings CallsignSettings { get; set; } = new CallsignSettings();

        /// <summary>
        /// Cycle settings
        /// </summary>
        public CycleSettings CycleSettings { get; set; } = new CycleSettings();

        /// <summary>
        /// Run times settings
        /// </summary>
        public RunTimesSettings RunTimesSettings { get; set; } = new RunTimesSettings();

        /// <summary>
        /// Fox power settings
        /// </summary>
        public PowerSettings PowerSettings { get; set; } = new PowerSettings();
    }
}
