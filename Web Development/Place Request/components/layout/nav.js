'use client'

import { Link } from '@chakra-ui/next-js'
import { Flex, Box, Button, IconButton, Spacer } from '@chakra-ui/react'
import { useSession } from 'next-auth/react'

import useSignInModal from '../shared/signInModal'
import UserDropdown from './userDropdown'
import { PlusSquareIcon } from '@chakra-ui/icons'
import { Suspense } from 'react'


export default function Nav() {
    const { SignInModal, setShowSignInModal } = useSignInModal();
    const { data: session } = useSession();

    const isAdmin = session?.user?.userType == 'Admin';

    return (
        <>
            <div
                className={`fixed top-0 w-full flex justify-center 
                    bg-white/50 backdrop-blur-xl
                    z-30 transition-all`}
            >
                <div className="mx-5 flex h-16 max-w-screen-xl items-center justify-between w-full">
                    <Link href="/" className="flex items-center font-display text-2xl"
                        style={{
                            textDecoration: 'none'
                        }}>
                        <p>好去处</p>
                    </Link>
                    <Suspense fallback={<></>}>
                    {session ? (
                        <Flex justify='flex-end' gap='10' >
                                {isAdmin ? (
                                    <></>
                                ) : (
                                <Link href="/request/create">
                                    <IconButton p={0}
                                bg="transparent"
                                icon={<PlusSquareIcon w='30px' h='30px' />} />
                                </Link>
                                )}
                            <UserDropdown />
                        </Flex>
                    ) : (
                        <div>
                                    <Button onClick={setShowSignInModal} colorScheme='telegram'>登录</Button>
                            <SignInModal />
                        </div>
                    )}
                    </Suspense>
                </div>
            </div>
        </>
    );
}