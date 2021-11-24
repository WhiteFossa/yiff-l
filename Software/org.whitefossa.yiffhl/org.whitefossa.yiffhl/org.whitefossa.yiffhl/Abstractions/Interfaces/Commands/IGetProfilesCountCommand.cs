namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetProfilesCountResponseDelegate(int count);

    public interface IGetProfilesCountCommand
    {
        void SetResponseDelegate(OnGetProfilesCountResponseDelegate onGetProfilesCountResponse);

        void SendGetProfilesCountCommand();
    }
}
