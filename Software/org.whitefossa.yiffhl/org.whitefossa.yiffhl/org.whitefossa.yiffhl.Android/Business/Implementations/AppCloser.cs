using Android.App;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    public class AppCloser : IAppCloser
    {
        public void Close()
        {
            var activity = (Activity)Forms.Context;
            activity.FinishAffinity();
        }
    }
}