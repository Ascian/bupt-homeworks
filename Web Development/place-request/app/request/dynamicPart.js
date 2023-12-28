'use client'

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
import { useRef, useEffect, useState } from 'react';
import { useToast } from '@chakra-ui/toast';
import { useSession } from 'next-auth/react';
import config from '@/app/config';
import CreateWelcome from '@/components/place-request/createWelcome';
import WelcomeCard from '@/components/place-request/welcomeCard';
import WelcomeCardPool from '@/components/place-request/welcomeCardPool';
import ModifyWelcome from "@/components/place-request/modifyWelcome";

export default async function DynamicPart({ requestId }) {
    const { data: session } = useSession();
    const { isOpen: isDeleteOpen, onOpen: onDeleteOpen, onClose: onDeleteClose } = useDisclosure()
    const [request, setRequest] = useState({});
    const [offers, setOffers] = useState([]);
    const [welcomes, setWelcomes] = useState([]);
    const [isRequester, setIsRequester] = useState(false);
    const [isOfferer, setIsOfferer] = useState(false);
    const [isReplied, setIsReplied] = useState(false);
    const [isLoading, setIsLoading] = useState(true);
    const cancelRef = useRef();
    const toast = useToast();

    useEffect(() => {
        let isRequestOk = false;
        let isOfferOk = false;
        let isWelcomeOk = false;
        fetch(`${config.serverIp}/seekers/${requestId}`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        })
            .then((res) => res.json())
            .then((request) => {
                if (request?.seekerId) {
                    setRequest(request);
                    isRequestOk = true;
                }
            });
        fetch(`${config.serverIp}/offers?user_id=${session?.user?.id}&seeker_id=${request.seekerId}&page=1&page_size=1`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        }).then((res) => res.json())
            .then((offerResponse) => {
                if (offerResponse?.data) {
                    setOffers(offerResponse.data);
                    isOfferOk = true;
                }
            });
        fetch(`${config.serverIp}/offers?seeker_id=${request.seekerId}&page=1&page_size=1`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        }).then((res) => res.json())
            .then((welcomeResponse) => {
                if (welcomeResponse?.data) {
                    setWelcomes(welcomeResponse.data);
                    isWelcomeOk = true;
                }   
            });
        setIsRequester(session?.user?.id === request.userId);
        setIsOfferer(offers?.length > 0);
        setIsReplied(welcomes?.length > 0);
        if (isRequestOk && isOfferOk && isWelcomeOk) {
            setIsLoading(false);
        }

    }, [session, requestId])

    // Fetch welcome to know if the request is replied

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


    const statusAlert = {
        'Active': 'info',
        'Accepted': 'success',
        'Completed': 'success',
        'Declined': 'error',
        'Cancelled': 'warning',
        'Expired': 'warning'
    }
    const status = {
        'Active': '已有他人回复等待确认',
        'Completed': '已完成',
        'Accepted': '已接受',
        'Declined': '已拒绝',
        'Cancelled': '已撤消',
        'Expired': '已过期'
    }

    if (isLoading) {
        return (
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        )
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

            <Box h='10' />
            <WelcomeCardPool requestId={requestId} isRequester={isRequester} isRequestActive={request.status === 'Active'} />

        </>
    );
}