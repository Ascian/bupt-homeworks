// auth.ts
import { getServerSession } from "next-auth/next";
import CredentialsProvider from "next-auth/providers/credentials";
import config from "@/app/config"

export const authOptions = {
    providers: [
        CredentialsProvider({
            name: "Credentials",
            credentials: {
                username: { label: "用户名", type: "text", placeholder: "请输入用户名" },
                password: { label: "密码", type: "password", placeholder: "请输入密码" }
            },
            async authorize(credentials, req) {

                const res = await fetch(`${config.serverIp}/users/login`, {
                    method: 'POST',
                    body: JSON.stringify({
                        username: credentials.username,
                        password: credentials.password
                    }),
                    headers: { "Content-Type": "application/json" }
                })
                const user = await res.json()
                // If no error and we have user data, return it
                if (res.ok && user) {
                    return user
                }
                return null
            }
        })
    ],
    callbacks: {
        async jwt({ token, user, account, trigger, session }) {
            // update() 
            if (trigger === "update") {
                token.user = session.user
                return token
            }
            // Initial sign in
            if (account && account.type === "credentials" && user) {
                const res = await fetch(`${config.serverIp}/users/me`, {
                    method: 'GET',
                    headers: {
                        "Authorization": `Bearer ${user.token}`,
                    },
                })
                const userProfile = await res.json()
                token.accessToken = user.token;

                const identityTypes = {
                    'IdCard': '居民身份证',
                    'Passport': '护照',
                }
                token.user = {
                    id: userProfile.id,
                    username: userProfile.username,
                    userType: userProfile.userType,
                    realName: userProfile.realName,
                    identityType: identityTypes[userProfile.documentType],
                    identityNumber: userProfile.documentNumber,
                    phoneNumber: userProfile.phoneNumber,
                    userLevel: userProfile.userLevel,
                    introduction: userProfile.bio,
                    city: userProfile.region,
                    province: userProfile.district,
                    country: userProfile.country,
                }
            }
            return token;
        },
        async session({ session, token }) {
            session.accessToken = token.accessToken;
            session.user = token.user;
            return session;
        },
    },
    secret: 'g1eWYZizPT3lsFO6TLeP9ydQ0LrlrNWCwXfedtOfQYc=',
}

export function auth(...args) {
    return getServerSession(...args, authOptions);
}