using System;
using Microsoft.SPOT;
using System.Ext;

namespace schemas.example.org.AttachmentService
{
    public class AttachmentServiceImplementation : IAttachmentService
    {
        public void OneWayAttachment(OneWayAttachmentRequest req)
        {
            System.Ext.Console.Write("OneWayAttachment received.");
            System.Ext.Console.Write("Attachment size = " + req.Param.Length);
            System.Ext.Console.Write("");
        }

        public TwoWayAttachmentResponse TwoWayAttachment(TwoWayAttachmentRequest req)
        {
            System.Ext.Console.Write("TwoWayAttachment received.");
            System.Ext.Console.Write("Attachment size = " + req.Param.Length);
            System.Ext.Console.Write("");
            TwoWayAttachmentResponse resp = new TwoWayAttachmentResponse();
            HelpIcon testIcon = new HelpIcon();
            resp.Param = testIcon.Data.ToArray();
            return resp;
        }
    }
}
