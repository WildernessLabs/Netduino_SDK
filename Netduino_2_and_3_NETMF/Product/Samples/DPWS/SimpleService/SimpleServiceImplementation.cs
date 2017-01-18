using System;
using Microsoft.SPOT;
using System.Ext;

namespace schemas.example.org.SimpleService
{
    class SimpleServiceImplementation : ISimpleService
    {
        public void OneWay(OneWayRequest req)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("OneWay request integer value = " 
                + req.Param.ToString());
        }

        public TwoWayResponse TwoWay(TwoWayRequest req)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("TwoWay request X = " + req.X.ToString() + " Y = " + req.Y.ToString());
            int Sum = req.X + req.Y;
            TwoWayResponse resp = new TwoWayResponse();
            resp.Sum = Sum;
            System.Ext.Console.Write("Sum = " + req.X.ToString() + " + " + req.Y.ToString() + " = " + Sum.ToString());
            return resp;
        }

        public TypeCheckResponse TypeCheck(TypeCheckRequest req)
        {
            TypeCheckResponse resp = new TypeCheckResponse();
            resp.BoolCheck = req.BoolCheck;
            resp.DecimalCheck = req.DecimalCheck;
            resp.FloatCheck = req.FloatCheck;
            resp.UriListCheck = new String[req.UriListCheck.Length];
            for (int i = 0; i < req.UriListCheck.Length; ++i)
            {
                resp.UriListCheck[i] = req.UriListCheck[i];   
            }
            return resp;
        }

        public AnyCheckResponse AnyCheck(AnyCheckRequest req)
        {
            AnyCheckResponse resp = new AnyCheckResponse();
            resp.Any = req.Any;
            resp.AnyAttr = req.AnyAttr;
            return resp;
        }

    }
}
