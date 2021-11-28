namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Frequency settings
    /// </summary>
    public class FrequencySettings
    {
        /// <summary>
        /// If true, then 2m (144-146MHz range will be used)
        /// </summary>
        public bool Is2m { get; set; }

        /// <summary>
        /// Frequency in Hz
        /// </summary>
        public int Frequency { get; set; }
    }
}
