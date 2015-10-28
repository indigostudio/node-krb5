#include <nan.h>
#include <string>
#include <cstring>
#include <node.h>
#include <v8.h>
extern "C" {
#include <krb5.h>
}

class krb5Worker: public Nan::AsyncWorker {
    public:
        krb5Worker(Nan::Callback *callback, const std::string& a_principal, const std::string& a_password):
            Nan::AsyncWorker(callback), m_principal(a_principal), m_password(a_password) {}

        ~krb5Worker() {}

        void Execute() {
            krb5_context context;
            krb5_principal principal;
            krb5_creds creds;

            int err;
            err = krb5_init_context(&context);
            if(err == 0)
            {
                err = krb5_parse_name(context, m_principal.c_str(), &principal);
                if(err == 0)
                {
                    err = krb5_get_init_creds_password(
                            context,
                            &creds,
                            principal,
                            (char*) m_password.c_str(),
                            NULL,
                            NULL,
                            0,
                            NULL,
                            NULL
                            );

                    if(err == 0)
                    {
                        krb5_free_cred_contents(context, &creds);
                    }

                    krb5_free_principal(context, principal);
                }

                if(err != 0) {
                    const char* msg = krb5_get_error_message(context, err);
                    SetErrorMessage(msg);
                    krb5_free_error_message(context, msg);
                }

                krb5_free_context(context);
            }
            else {
                SetErrorMessage(std::strerror(err));
            }
        }

    private:
        std::string m_principal;
        std::string m_password;
};

NAN_METHOD(authenticate) {
    Nan::HandleScope scope;

	if(info.Length() < 3)
	{
		Nan::ThrowError("too few arguments");
        return;
	}

	if(!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsFunction())
	{
        Nan::ThrowError("expected a principal, password and callback as arguments");
        return;
	}

    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    std::string principal(*Nan::Utf8String(info[0]));
    std::string password(*Nan::Utf8String(info[1]));

    Nan::AsyncQueueWorker(new krb5Worker(callback, principal, password));
    info.GetReturnValue().SetUndefined();
}


NAN_MODULE_INIT(Init) {
    NAN_EXPORT(target, authenticate);
}

NODE_MODULE(krb5, Init);

