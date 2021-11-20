using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;

namespace org.whitefossa.yiffhl.Business.Implementations.Events
{
    public class AntennaMatchingMeasurementEvent : IAntennaMatchingMeasurementEvent
    {
        private readonly int MatchingPosition;
        private readonly float AntennaVoltage;

        private AntennaMatchingMeasurementEvent()
        {
        }

        public AntennaMatchingMeasurementEvent(int matchingPosition, float antennaVoltage)
        {
            MatchingPosition = matchingPosition;
            AntennaVoltage = antennaVoltage;
        }

        public float GetAntennaVoltage()
        {
            return AntennaVoltage;
        }

        public int GetMatchingPosition()
        {
            return MatchingPosition;
        }
    }
}
