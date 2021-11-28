using System;
using System.Collections.Generic;
using System.Text;

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
    }
}
