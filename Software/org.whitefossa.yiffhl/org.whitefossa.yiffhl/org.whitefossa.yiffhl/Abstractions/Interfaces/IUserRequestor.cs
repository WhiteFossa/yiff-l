using Acr.UserDialogs;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to show various user dialogs, not related to notifications.
    /// For notifications use IUserNotifier
    /// </summary>
    public interface IUserRequestor
    {
        /// <summary>
        /// Shows string input dialog.
        /// Returns true and string if user entered a string, returns false and meaningless value if not
        /// </summary>
        /// <returns></returns>
        Task<(bool, string)> EnterStringAsync
        (
            string title,
            string message,
            string initialValue,
            int maxLength,
            Action<PromptTextChangedArgs> onTextChanged
        );
    }
}
