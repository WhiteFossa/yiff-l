﻿using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetDateAndTimeResponseDelegate(bool isSuccessfull);

    public interface ISetDateAndTimeCommand
    {
        void SetResponseDelegate(OnSetDateAndTimeResponseDelegate onSetDateAndTimeResponse);

        void SendSetDateAndTimeCommand(DateTime time);
    }
}
