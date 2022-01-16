using System;
using System.Collections.Generic;
using System.Text;

namespace org.whitefossa.yiffhl.Models
{
    public class ServiceSettingsModel
    {
        /// <summary>
        /// Last error code, reported by fox
        /// </summary>
        public uint? LastErrorCode { get; set; }

        /// <summary>
        /// Battery averaged ADC level
        /// </summary>
        public float BatteryAveragedADCLevel { get; set; }

        public ServiceSettingsModel()
        {
            LastErrorCode = null;
        }
    }
}
