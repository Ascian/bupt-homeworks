'use client'

import {
    Box,
    Flex,
    Stack,
    Text,
    Heading,
    Image,
    Card,
    CardBody,
    Button,
    AlertDialog,
    AlertDialogBody,
    AlertDialogFooter,
    AlertDialogHeader,
    AlertDialogContent,
    AlertDialogOverlay,
    Link,


} from "@chakra-ui/react";
import { useDisclosure } from "@chakra-ui/hooks";
import { useToast } from "@chakra-ui/toast";
import { useRef } from "react";
import { useSession } from "next-auth/react";
import config from "@/app/config";

export default function WelcomeCard({ welcome, showButton, isWelcomeAccepted }) {
    const date = new Date(welcome.updateTime).toLocaleDateString()
    const { isOpen: isAcceptOpen, onOpen: onAcceptOpen, onClose: onAcceptClose } = useDisclosure()
    const { isOpen: isDeclineOpen, onOpen: onDeclineOpen, onClose: onDeclineClose } = useDisclosure()
    const cancelRef = useRef();
    const { data: session } = useSession();
    const isAdmin = session?.user?.userType == 'Admin';

    const toast = useToast()
    const handleAccept = async () => {

        const res = await fetch(`${config.serverIp}/offers/${welcome.offerId}/accept`, {
            method: 'POST',
            headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${session.accessToken}`
            },
        })
        if (res.ok) {

            window.location.reload();
            return;
        }
        else {
            const response = await res.json()
            toast({
                title: '接受失败',
                description: response.errorMessage,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    }

    const handleDecline = async () => {

        const res = await fetch(`${config.serverIp}/offers/${welcome.offerId}/decline`, {
            method: 'POST',
            headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${session.accessToken}`
            },
        })
        if (res.ok) {

            window.location.reload();
            return;
        }
        else {
            const response = await res.json()
            toast({
                title: '拒绝失败',
                description: response.errorMessage,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    }

    return (
        <div>
            <Card align='center' w='750px' h='auto' bg="transparent" boxShadow="none">
                <CardBody>
                    <Flex w='700px' >
                        {isAdmin ? (
                            <Link href={`/manager/users/user?user_id=${welcome.userId}`} style={{ textDecoration: 'none' }}>
                        <Image borderRadius="full" boxSize="40px" src="/userIcon.png" alt="User Icon" />
                        </Link>
                        ) : (
                            <Image borderRadius="full" boxSize="40px" src="/userIcon.png" alt="User Icon" />
                        )}
                        <Box w="4" />
                        <Flex w='600px' justify='flex-start' align='center' >
                            <Stack >
                                {isAdmin ? (
                                    <Link href={`/manager/users/user?user_id=${welcome.userId}`} style={{ textDecoration: 'none' }}>
                                <Heading fontSize='md' >{welcome.username}</Heading>
                                    </Link>
                                ) : (
                                    <Heading fontSize='md' >{welcome.username}</Heading>
                                )}
                                <Text fontSize='md' >{welcome.offerDescription}</Text>
                                <Text fontSize='sm' color='grey' >{date}</Text>
                            </Stack>
                        </Flex>
                        {showButton ? (
                            <>
                                <Flex w='100px' justify='flex-end' align='center' >
                                    <Stack>
                                        <Button colorScheme='facebook' onClick={onAcceptOpen} >
                                            接受
                                        </Button>
                                        <AlertDialog
                                            isOpen={isAcceptOpen}
                                            leastDestructiveRef={cancelRef}
                                            onClose={onAcceptClose}
                                        >
                                            <AlertDialogOverlay>
                                                <AlertDialogContent>
                                                    <AlertDialogHeader fontSize='lg' fontWeight='bold'>
                                                        接受回复
                                                    </AlertDialogHeader>

                                                    <AlertDialogBody>
                                                        你确定要接受这条回复吗？此操作不可撤销。
                                                    </AlertDialogBody>

                                                    <AlertDialogFooter>
                                                        <Button ref={cancelRef} onClick={() => { onAcceptClose(); window.location.reload(); }}>
                                                            取消
                                                        </Button>
                                                        <Button colorScheme='red' onClick={() => { handleAccept(); onAcceptClose(); }} ml={3}>
                                                            接受
                                                        </Button>
                                                    </AlertDialogFooter>
                                                </AlertDialogContent>
                                            </AlertDialogOverlay>
                                        </AlertDialog>
                                        <Button colorScheme='red' onClick={onDeclineOpen}>
                                            拒绝
                                        </Button>
                                        <AlertDialog
                                            isOpen={isDeclineOpen}
                                            leastDestructiveRef={cancelRef}
                                            onClose={onDeclineClose}
                                        >
                                            <AlertDialogOverlay>
                                                <AlertDialogContent>
                                                    <AlertDialogHeader fontSize='lg' fontWeight='bold'>
                                                        拒绝回复
                                                    </AlertDialogHeader>

                                                    <AlertDialogBody>
                                                        你确定要拒绝这条回复吗？此操作不可撤销。
                                                    </AlertDialogBody>

                                                    <AlertDialogFooter>
                                                        <Button ref={cancelRef} onClick={() => { onDeclineClose(); window.location.reload(); }}>
                                                            取消
                                                        </Button>
                                                        <Button colorScheme='red' onClick={() => { handleDecline(); onDeleteDecline(); }} ml={3}>
                                                            拒绝
                                                        </Button>
                                                    </AlertDialogFooter>
                                                </AlertDialogContent>
                                            </AlertDialogOverlay>
                                        </AlertDialog>
                                    </Stack>
                                </Flex>
                            </>
                        ) : (
                                <>
                                {isWelcomeAccepted ? (
                                    <Flex w='100px' justify='flex-end' align='center' >
                                        <Text fontSize='lg' color='green' >已接受</Text>
                                    </Flex>
                                ) : (
                                    <></>
                                )}
                            </>
                        )}
                    </Flex>

                </CardBody>
            </Card>
        </div>
    )
}   