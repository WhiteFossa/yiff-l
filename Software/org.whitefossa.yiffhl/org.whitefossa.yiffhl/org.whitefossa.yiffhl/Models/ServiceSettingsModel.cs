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

        /// <summary>
        /// Battery level = UBatt(Volts) * A + B
        /// </summary>
        public float BattLevelFactorA { get; set; }

        /// <summary>
        /// Battery level = UBatt(Volts) * A + B
        /// </summary>
        public float BattLevelFactorB { get; set; }

        /// <summary>
        /// U80m averaged ADC value
        /// </summary>
        public float U80mAveragedADCValue { get; set; }

        /// <summary>
        /// U80m averaged voltage level
        /// </summary>
        public float U80mAveragedVoltage { get; set; }

        /// <summary>
        /// U80m(Volts) = U80m(ADC) * A + B
        /// </summary>
        public float U80mFactorA { get; set; }

        /// <summary>
        /// U80m(Volts) = U80m(ADC) * A + B
        /// </summary>
        public float U80mFactorB { get; set; }

        public ServiceSettingsModel()
        {
            LastErrorCode = null;

            BatteryAveragedADCLevel = 0;

            BatteryAveragedVoltageLevel = 0;

            BattLevelFactorA = 0;
            BattLevelFactorB = 0;

            U80mAveragedADCValue = 0;
            U80mAveragedVoltage = 0;

            U80mFactorA = 0;
            U80mFactorB = 0;
        }
    }
}
