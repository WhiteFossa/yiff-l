namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSerialNumberUpdatedDelegate();

    public interface IUpdateSerialNumberCommand
    {
        void SetResponseDelegate(OnSerialNumberUpdatedDelegate onSerialNumberUpdated);

        void SendUpdateSerialNumberCommand(uint newSerialNuber);
    }
}
