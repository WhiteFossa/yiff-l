namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetProfileNameResponseDelegate(bool isSuccessful);

    public interface ISetProfileNameCommand
    {
        void SetResponseDelegate(OnSetProfileNameResponseDelegate onSetProfileNameResponse);

        void SendSetProfileNameCommand(string name);
    }
}
