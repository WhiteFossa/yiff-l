using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class UserRequestor : IUserRequestor
    {
        public async Task<(bool, string)> EnterStringAsync(string title, string message, string initialValue, int maxLength)
        {
            var config = new PromptConfig
            {
                Title = title,
                Message = message,
                Text = initialValue,
                MaxLength = maxLength
            };
            var result = await UserDialogs.Instance.PromptAsync(config);

            if (result.Ok)
            {
                return (true, result.Text);
            }

            return (false, string.Empty);
        }
    }
}
