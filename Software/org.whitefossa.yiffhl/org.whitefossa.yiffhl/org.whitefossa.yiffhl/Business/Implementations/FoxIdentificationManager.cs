using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxIdentificationManager : IFoxIdentificationManager
    {
        private readonly IGetIdentificationDataCommand _getIdentificationDataCommand;

        private OnFoxIdentificationDelegate _onFoxIdentification;

        public FoxIdentificationManager(IGetIdentificationDataCommand getIdentificationDataCommand)
        {
            _getIdentificationDataCommand = getIdentificationDataCommand;
        }

        public async Task IdentifyFoxAsync(OnFoxIdentificationDelegate onFoxIdentification)
        {
            _onFoxIdentification = onFoxIdentification ?? throw new ArgumentNullException(nameof(onFoxIdentification));

            _getIdentificationDataCommand.SetResponseDelegate(OnGetIdentification);
            _getIdentificationDataCommand.SendGetIdentificationDataCommand();
        }

        private void OnGetIdentification
        (
            bool isFox,
            UInt16 protocolVersion,
            UInt16 hardwareRevision,
            UInt16 softwareVersion,
            UInt32 serialNumber
         )
        {
            _onFoxIdentification(isFox, protocolVersion, hardwareRevision, softwareVersion, serialNumber);
        }
    }
}
