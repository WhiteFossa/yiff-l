namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetFoxNameResponseDelegate(bool isSuccessful);

    public interface ISetFoxNameCommand
    {
        void SetResponseDelegate(OnSetFoxNameResponseDelegate onSetFoxNameResponse);

        void SendSetFoxNameCommand(string name);
    }
}
