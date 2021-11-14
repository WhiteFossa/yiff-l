using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to show alerts and notifications to an user
    /// </summary>
    public interface IUserNotifier
    {
        /// <summary>
        /// Show error message to an user
        /// </summary>
        Task ShowErrorMessageAsync(string title, string message);
    }
}
