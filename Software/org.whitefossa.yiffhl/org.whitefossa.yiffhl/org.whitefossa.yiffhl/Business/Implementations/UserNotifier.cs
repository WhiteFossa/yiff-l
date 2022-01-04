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

        public async Task ShowNotificationMessageAsync(string title, string message)
        {
            await UserDialogs.Instance.AlertAsync(message, title, "OK");
        }

        public async Task<bool> ShowYesNoRequestAsync(string title, string message)
        {
            return await UserDialogs.Instance.ConfirmAsync(message, title, "Yes", "No");
        }
    }
}
