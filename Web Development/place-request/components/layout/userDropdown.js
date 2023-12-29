'use client'

import {
    Popover,
    PopoverTrigger,
    PopoverContent,
    PopoverBody,
    PopoverArrow,
    PopoverCloseButton,
    Button,
    IconButton,
    Stack,
    Text,
    Link,
} from '@chakra-ui/react'
import UserIcon from '../shared/userIcon'
import { useSession, signOut } from 'next-auth/react'

export default function UserDropdown() {
    const { data: session } = useSession();
    const isAdmin = session?.user?.userType == 'Admin';

    return (
        <Popover placement="bottom-end">
            <PopoverTrigger>
                <IconButton
                    icon={<UserIcon />}
                    p={0}
                    bg="transparent"
                    _hover={{ bg: 'transparent' }}
                />
            </PopoverTrigger>
            <PopoverContent
                zIndex={100}
                boxShadow='xl'
                w="auto"
                minW="200px"
                maxW="300px"
            >
                <PopoverArrow />
                <PopoverCloseButton />
                <PopoverBody p={4}>
                    <Link href={`/user`}>
                        <Button
                            w="full"
                            h='80px'
                            p={0}
                            bg="transparent"
                            borderRadius="md"
                            mt={2}
                            mb={0}
                            _hover={{ bg: 'gray.100' }}
                            textAlign="left"
                            justifyContent="flex-start"
                        >
                            <Stack spacing={0}>
                                <Text fontSize='3xl'>{session.user.username}</Text>
                                <Text fontSize='1xl'>{`${session.user.city}/${session.user.province}/${session.user.country}`} </Text>
                            </Stack>
                        </Button>
                    </Link>
                    {isAdmin ? (
                        <>
                            <Link href={`/manager`}>
                                <Button
                                    w="full"
                                    p={0}
                                    bg="transparent"
                                    borderRadius="md"
                                    mb={0}
                                    _hover={{ bg: 'gray.100' }}
                                    textAlign="left"
                                    justifyContent="flex-start"
                                >
                                    统计信息
                                </Button>
                            </Link>
                            <Link href={`/manager/users`}>
                                <Button
                                    w="full"
                                    p={0}
                                    bg="transparent"
                                    borderRadius="md"
                                    mb={0}
                                    _hover={{ bg: 'gray.100' }}
                                    textAlign="left"
                                    justifyContent="flex-start"
                                >
                                    用户信息
                                </Button>
                            </Link>
                        </>
                    ) : (
                        <>
                                <Link href={`/user/requests`}>
                                    <Button
                                        w="full"
                                        p={0}
                                        bg="transparent"
                                        borderRadius="md"
                                        mb={0}
                                        _hover={{ bg: 'gray.100' }}
                                        textAlign="left"
                                        justifyContent="flex-start"
                                    >
                                        我请求的
                                    </Button>
                                </Link>
                                <Link href={`/user/offers`}>
                                    <Button
                                        w="full"
                                        p={0}
                                        bg="transparent"
                                        borderRadius="md"
                                        mb={0}
                                        _hover={{ bg: 'gray.100' }}
                                        textAlign="left"
                                        justifyContent="flex-start"
                                    >
                                        我回复的
                                    </Button>
                                </Link>
                        </>
                    )}
                    <Button
                        w="full"
                        p={0}
                        bg="transparent"
                        borderRadius="md"
                        _hover={{ bg: 'gray.100' }}
                        textAlign="left"
                        justifyContent="flex-start"
                        onClick={() => signOut()}
                    >
                        登出
                    </Button>
                </PopoverBody>
            </PopoverContent>
        </Popover >
    );
}