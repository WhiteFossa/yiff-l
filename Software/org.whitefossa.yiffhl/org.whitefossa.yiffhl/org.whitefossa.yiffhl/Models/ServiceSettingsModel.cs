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

        /// <summary>
        /// Battery averaged voltage level
        /// </summary>
        public float BatteryAveragedVoltageLevel { get; set; }

        /// <summary>
        /// UBatt(Volts) = UBatt(ADC) * A + B
        /// </summary>
        public float UBattFactorA { get; set; }

        /// <summary>
        /// UBatt(Volts) = UBatt(ADC) * A + B
        /// </summary>
        public float UBattFactorB { get; set; }

        public ServiceSettingsModel()
        {
            LastErrorCode = null;
            BatteryAveragedADCLevel = 0;
            BatteryAveragedVoltageLevel = 0;
        }
    }
}
