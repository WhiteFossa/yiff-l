using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Frequency settings
    /// </summary>
    public class FrequencySettings : ICloneable
    {
        /// <summary>
        /// If true, then 2m (144-146MHz range will be used)
        /// </summary>
        public bool Is2m { get; set; }

        /// <summary>
        /// Frequency in Hz
        /// </summary>
        public int Frequency { get; set; }

        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}
