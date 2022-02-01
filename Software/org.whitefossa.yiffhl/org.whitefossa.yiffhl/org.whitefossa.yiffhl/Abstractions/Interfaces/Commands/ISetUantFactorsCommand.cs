﻿namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetUantFactorsResponseDelegate(bool isSuccessful);

    public interface ISetUantFactorsCommand
    {
        void SetResponseDelegate(OnSetUantFactorsResponseDelegate onSetUantFactorsResponse);

        void SendSetUantFactors(bool resetToDefaults, float a, float b);
    }
}
