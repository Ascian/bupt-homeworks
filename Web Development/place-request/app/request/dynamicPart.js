import {
    Button,
    Card,
    CardBody,
    Flex,
    Box,
    Heading,
    Divider,
    Alert,
    AlertIcon,
    AlertTitle,
    Spinner,
    AlertDialog,
    AlertDialogOverlay,
    AlertDialogContent,
    AlertDialogHeader,
    AlertDialogFooter,
    AlertDialogBody,
    Link,

} from "@chakra-ui/react";
import { useDisclosure } from "@chakra-ui/hooks";
import { useRef } from 'react';
import { useToast } from '@chakra-ui/toast';
import { useSession } from 'next-auth/react';
import config from '@/app/config';
import CreateWelcome from '@/components/place-request/createWelcome';
import WelcomeCard from '@/components/place-request/welcomeCard';
import ModifyWelcome from "@/components/place-request/modifyWelcome";

export default async function DynamicPart({ requestId }) {
    const { data: session } = useSession();
    const { isOpen: isDeleteOpen, onOpen: onDeleteOpen, onClose: onDeleteClose } = useDisclosure()
    const cancelRef = useRef();
    const toast = useToast();

    // Fetch request
    const requestRes = await fetch(`${config.serverIp}/seekers/${requestId}`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const request = await requestRes.json()
    console.log(request)
    if (!requestRes.ok || !request) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }

    // Fetch offers by the user
    const offerRes = await fetch(`${config.serverIp}/offers?user_id=${session?.user?.id}&seeker_id=${request.seekerId}&page=1&page_size=1`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const offerResponse = await offerRes.json()
    if (!offerRes.ok || !offerResponse?.data) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }
    const offers = offerResponse.data

    // Fetch welcome to know if the request is replied
    const welcomeRes = await fetch(`${config.serverIp}/offers?seeker_id=${request.seekerId}&page=1&page_size=1`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const welcomeResponse = await welcomeRes.json()
    if (!welcomeRes.ok || !welcomeResponse?.data) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }
    const welcomes = welcomeResponse.data

    const handleDelete = async () => {
        const res = await fetch(`${config.serverIp}/seekers/${requestId}`, {
            method: 'DELETE',
            headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${session.accessToken}`
            },
        })

        // If no error and we have user data, return it
        if (res.ok) {
            toast({
                title: '删除成功',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            window.location.reload();
            return;
        }
        // Return null if user data could not be retrieved
        else {
            const response = await res.json()
            toast({
                title: '删除失败',
                description: `state: ${res.status}, message: ${response.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    }

    const isRequester = session?.user?.id === request.userId;
    const isOfferer = offers.length > 0;
    const isReplied = welcomes.length > 0;

    const statusAlert = {
        'Active': 'info',
        'Accepted': 'success',
        'Declined': 'error',
        'Cancelled': 'warning',
        'Expired': 'warning'
    }
    const status = {
        'Active': '已有他人回复等待确认',
        'Accepted': '已接受',
        'Declined': '已拒绝',
        'Cancelled': '已撤消',
        'Expired': '已过期'
    }
    return (
        <>
            {
                session ? (
                    <>
                        {
                            isRequester ? (
                                <>
                                    {
                                        request.status === 'Active' && !isReplied ? (
                                            <Card w='800px'>
                                                <CardBody>
                                                    <Flex w='800px' justify='center' >
                                                        <Link href={`/request/modify?request_id=${requestId}`}
                                                            style={{ textDecoration: 'none' }}
                                                        >
                                                            <Button colorScheme='facebook' >修改请求</Button>
                                                        </Link>
                                                        <Box w='4' />
                                                        <Button colorScheme='red' onClick={onDeleteOpen}>删除请求</Button>
                                                        <AlertDialog
                                                            isOpen={isDeleteOpen}
                                                            leastDestructiveRef={cancelRef}
                                                            onClose={onDeleteClose}
                                                        >
                                                            <AlertDialogOverlay>
                                                                <AlertDialogContent>
                                                                    <AlertDialogHeader fontSize='lg' fontWeight='bold'>
                                                                        删除请求
                                                                    </AlertDialogHeader>

                                                                    <AlertDialogBody>
                                                                        你确定要删除这条请求吗？此操作不可撤销。
                                                                    </AlertDialogBody>

                                                                    <AlertDialogFooter>
                                                                        <Button ref={cancelRef} onClick={onDeleteClose}>
                                                                            取消
                                                                        </Button>
                                                                        <Button colorScheme='red' onClick={() => { handleDelete(); onDeleteClose(); }} ml={3}>
                                                                            删除
                                                                        </Button>
                                                                    </AlertDialogFooter>
                                                                </AlertDialogContent>
                                                            </AlertDialogOverlay>
                                                        </AlertDialog>
                                                    </Flex>
                                                </CardBody>
                                            </Card>
                                        ) : (
                                            <>
                                                <Card align='left' w='800px'>
                                                    <Alert status={statusAlert[request.status]}>
                                                        <AlertIcon />
                                                        <AlertTitle>该请求{status[request.status]}</AlertTitle>
                                                    </Alert >
                                                </Card >
                                            </>
                                        )
                                    }
                                </>
                            ) : (
                                <>
                                    {
                                        isOfferer ? (
                                            <Card w='800px' h='auto' >
                                                <CardBody>
                                                    <Heading fontSize='30px' w='750px' textAlign='center' >我的回复</Heading>
                                                    <Box h='2' />
                                                    <Divider w='750px' />
                                                    {offers.map((welcome) => (
                                                        <>
                                                            {welcome.status === 'Active' ? (
                                                                <ModifyWelcome welcome={welcome} />
                                                            ) : (
                                                                <>
                                                                    <WelcomeCard welcome={welcome} />
                                                                    <Box h='2' />
                                                                    <Card align='left' w='750px'>
                                                                        <Alert status={statusAlert[welcome.status]}>
                                                                            <AlertIcon />
                                                                            <AlertTitle>该回复{status[welcome.status]}</AlertTitle>
                                                                        </Alert >
                                                                    </Card >
                                                                </>
                                                            )}
                                                        </>

                                                    ))}
                                                </CardBody >
                                            </Card >
                                        ) : (
                                            <CreateWelcome requestId={requestId} userId={session.user.userId} />
                                        )}
                                </>
                            )
                        }

                    </>
                ) : (
                    <Card align='left' w='800px'>
                        <Alert status='error'>
                            <AlertIcon />
                            <AlertTitle>登录后可回复</AlertTitle>
                        </Alert >
                    </Card >
                )
            }

        </>
    );
}