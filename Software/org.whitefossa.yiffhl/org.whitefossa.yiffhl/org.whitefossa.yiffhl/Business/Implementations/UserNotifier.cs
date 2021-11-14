using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class UserNotifier : IUserNotifier
    {
        public async Task ShowErrorMessageAsync(string title, string message)
        {
            await UserDialogs.Instance.AlertAsync(message, title, "OK");
        }
    }
}
