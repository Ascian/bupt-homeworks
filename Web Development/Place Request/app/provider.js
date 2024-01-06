'use client'
import { ChakraProvider } from '@chakra-ui/react'
import { SessionProvider } from "next-auth/react"

export function Providers({ children, pageProps: session, ...pageProps }) {
    return (
        <SessionProvider session={session}>
            <ChakraProvider>
                {children}
            </ChakraProvider>
        </SessionProvider>
    );
}