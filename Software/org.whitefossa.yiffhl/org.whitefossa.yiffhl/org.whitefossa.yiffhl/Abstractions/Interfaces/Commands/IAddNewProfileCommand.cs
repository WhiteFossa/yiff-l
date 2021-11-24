namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnAddNewProfileResponseDelegate(bool isSuccessful);

    public interface IAddNewProfileCommand
    {
        void SetResponseDelegate(OnAddNewProfileResponseDelegate onAddNewProfileResponse);

        void SendAddNewProfileCommand();
    }
}
