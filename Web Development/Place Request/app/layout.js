
import './globals.css'
import { Providers } from './provider'
import Nav from '@/components/layout/nav'

export default function RootLayout({ children }) {
  return (
    <html>
      <body className='text-black'>
        <Providers>
          <div className="fixed h-screen w-full bg-gradient-to-br from-indigo-50 via-white to-cyan-100" />
          <Nav />
          <main className="flex min-h-screen w-full flex-col items-center justify-center py-32">
            {children}
          </main>
        </Providers>
      </body>
    </html>
  )
}
