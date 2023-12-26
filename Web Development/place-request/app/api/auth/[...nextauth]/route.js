import NextAuth from "next-auth"
import CredentialsProvider from "next-auth/providers/credentials";
import config from '@/app/config'

const handler = NextAuth({
    providers: [
        CredentialsProvider({
            name: "Credentials",
            credentials: {
                username: { label: "用户名", type: "text", placeholder: "请输入用户名" },
                password: { label: "密码", type: "password", placeholder: "请输入密码" }
            },
            async authorize(credentials, req) {
                const account = {
                    username: credentials.username,
                    password: credentials.password,
                }

                const res = await fetch(`${config.serverIp}/users/login`, {
                    method: 'POST',
                    body: JSON.stringify(account),
                    headers: { "Content-Type": "application/json" }
                })
                const user = await res.json()
                // const user = {
                //     username: "username",
                //     realName: "realName",
                //     identityType: "identityType",
                //     identityNumber: "identityNumber",
                //     phoneNumber: "phoneNumber",
                //     introduction: "introduction",
                //     city: "city",
                //     province: "province",
                //     country: "country",
                // };
                // If no error and we have user data, return it
                if (res.ok && user) {
                    return user
                }
                // Return null if user data could not be retrieved
                return null

            }
        })
    ],
    callbacks: {
        async jwt({ token, user, account }) {
            if (account && account.type === "credentials" && user) {
                token.user = user;
                token.accessToken = user.access_token;
            }
            return token;
        },
        async session({ session, token }) {
            session.user = token.user;
            session.accessToken = token.accessToken;
            return session;
        },
    }
})

export { handler as GET, handler as POST }