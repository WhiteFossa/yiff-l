namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetFoxNameResponseDelegate(string name);

    public interface IGetFoxNameCommand
    {
        void SetResponseDelegate(OnGetFoxNameResponseDelegate onGetFoxNameResponse);

        void SendGetFoxNameCommand();
    }
}
