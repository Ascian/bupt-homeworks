import NextAuth from "next-auth"
import CredentialsProvider from "next-auth/providers/credentials";
import config from '@/app/config'

const handler = NextAuth({
    providers: [
        CredentialsProvider({
            name: 'Credentials',
            credentials: {
                username: { label: "用户名", type: "text", placeholder: "请输入用户名" },
                password: { label: "密码", type: "password", placeholder: "请输入密码" }
            },
            async authorize(credentials, req) {


                // const res = await fetch(`${config.serverIp}/users/login`, {
                //     method: 'POST',
                //     body: JSON.stringify(credentials),
                //     headers: { "Content-Type": "application/json" }
                // })
                // const user = await res.json()

                // // If no error and we have user data, return it
                // if (res.ok && user) {
                //     return user
                // }
                // // Return null if user data could not be retrieved
                // return null
                const user = {
                    username: "username",
                    realName: "realName",
                    identityType: "identityType",
                    identityNumber: "identityNumber",
                    phoneNumber: "phoneNumber",
                    introduction: "introduction",
                    city: "city",
                    province: "province",
                    country: "country",
                };
                return user;
            }
        })
    ],
})

export { handler as GET, handler as POST }