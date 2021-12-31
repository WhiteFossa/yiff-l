namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnCheckForProfileSettingsChangesDelegate(bool isChangesExist);

    public interface ICheckForProfileSettingsChangesCommand
    {
        void SetResponseDelegate(OnCheckForProfileSettingsChangesDelegate onCheckForProfileSettingsChanges);

        void SendCheckForProfileSettingsChangesCommand();
    }
}
