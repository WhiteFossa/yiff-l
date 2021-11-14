﻿using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class UserNotifier : IUserNotifier
    {
        public async Task ShowErrorMessageAsync(string title, string message)
        {
            await Application.Current.MainPage.DisplayAlert(title, message, "Ok");
        }
    }
}
