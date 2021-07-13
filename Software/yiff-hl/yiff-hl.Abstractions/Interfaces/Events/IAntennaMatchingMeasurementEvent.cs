namespace yiff_hl.Abstractions.Interfaces.Events
{
    public interface IAntennaMatchingMeasurementEvent
    {
        int GetMatchingPosition();

        float GetAntennaVoltage();
    }
}
