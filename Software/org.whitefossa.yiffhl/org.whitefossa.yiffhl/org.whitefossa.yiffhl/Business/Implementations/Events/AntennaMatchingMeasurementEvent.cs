using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;

namespace org.whitefossa.yiffhl.Business.Implementations.Events
{
    public class AntennaMatchingMeasurementEvent : IAntennaMatchingMeasurementEvent
    {
        private readonly int _matchingPosition;
        private readonly int _totalMatchingPositions;
        private readonly float _antennaVoltage;

        private AntennaMatchingMeasurementEvent()
        {
        }

        public AntennaMatchingMeasurementEvent(int matchingPosition, int totalMatchingPositions, float antennaVoltage)
        {
            _matchingPosition = matchingPosition;
            _totalMatchingPositions = totalMatchingPositions;
            _antennaVoltage = antennaVoltage;
        }

        public float GetAntennaVoltage()
        {
            return _antennaVoltage;
        }

        public int GetMatchingPosition()
        {
            return _matchingPosition;
        }

        public int GetTotalMatchingPositionsCount()
        {
            return _totalMatchingPositions;
        }
    }
}
