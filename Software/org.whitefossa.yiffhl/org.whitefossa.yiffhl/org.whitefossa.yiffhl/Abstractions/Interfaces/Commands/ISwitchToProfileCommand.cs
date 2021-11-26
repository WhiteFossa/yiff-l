﻿namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSwitchToProfileResponseDelegate(bool isSuccessfull);

    public interface ISwitchToProfileCommand
    {
        void SetResponseDelegate(OnSwitchToProfileResponseDelegate onSwitchToProfileResponse);

        void SendSwitchToProfileCommand(int id);
    }
}