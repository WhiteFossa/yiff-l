namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Events
{
    public interface IAntennaMatchingMeasurementEvent
    {
        int GetMatchingPosition();

        float GetAntennaVoltage();
    }
}
