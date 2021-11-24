namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetProfileNameResponseDelegate(bool isSuccessful, int profileId, string name);

    public interface IGetProfileNameCommand
    {
        void SetResponseDelegate(OnGetProfileNameResponseDelegate onGetProfileNameResponse);

        void SendGetProfileNameCommand(int id);
    }
}
