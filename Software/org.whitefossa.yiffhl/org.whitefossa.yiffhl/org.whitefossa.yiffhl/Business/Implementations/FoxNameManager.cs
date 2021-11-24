using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxNameManager : IFoxNameManager
    {
        private readonly IGetFoxNameCommand _getFoxNameCommand;
        private readonly ISetFoxNameCommand _setFoxNameCommand;

        private OnGetNameDelegate _onGetName;
        private OnSetNameDelegate _onSetName;

        public FoxNameManager(IGetFoxNameCommand getFoxNameCommand,
            ISetFoxNameCommand setFoxNameCommand)
        {
            _getFoxNameCommand = getFoxNameCommand;
            _setFoxNameCommand = setFoxNameCommand;
        }

        public async Task GetNameAsync(OnGetNameDelegate onGetName)
        {
            _onGetName = onGetName ?? throw new ArgumentNullException(nameof(onGetName));

            _getFoxNameCommand.SetResponseDelegate(OnGetFoxNameResponse);
            _getFoxNameCommand.SendGetFoxNameCommand();
        }

        public async Task SetNameAsync(string name, OnSetNameDelegate onSetName)
        {
            _onSetName = onSetName ?? throw new ArgumentNullException(nameof(onSetName));

            _setFoxNameCommand.SetResponseDelegate(OnSetFoxNameResponse);
            _setFoxNameCommand.SendSetFoxNameCommand(name);
        }

        private void OnGetFoxNameResponse(string name)
        {
            _onGetName(name);
        }

        private void OnSetFoxNameResponse(bool isSuccessfull)
        {
            _onSetName(isSuccessfull);
        }
    }
}
