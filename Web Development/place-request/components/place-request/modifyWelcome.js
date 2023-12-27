'use client'

import {
    Box,
    Card,
    CardBody,
    Flex,
    Textarea,
    Button,
    AlertDialog,
    AlertDialogBody,
    AlertDialogFooter,
    AlertDialogHeader,
    AlertDialogContent,
    AlertDialogOverlay,
    Image,
    Stack,
    Heading,

} from "@chakra-ui/react";
import { useDisclosure } from "@chakra-ui/hooks";
import { useToast } from '@chakra-ui/toast';
import { useState, useRef } from 'react';
import { useSession } from 'next-auth/react';
import config from '@/app/config'

export default async function ModifyWelcome({ welcome }) {
    const [description, setDescription] = useState('');
    const { isOpen, onOpen, onClose } = useDisclosure()
    const cancelRef = useRef()
    const { data: session } = useSession();
    const toast = useToast()

    const handleModify = async () => {
        if (description === '' || description.length > 10000) {
            toast({
                title: '描述不合法',
                description: "描述不能为空且长度不能超过10000",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsDescriptionInvalid(true);
            return;
        }

        const res = await fetch(`${config.serverIp}/offers/${welcome.offerId}`, {
            method: 'PATCH',
            body: JSON.stringify({
                offerDescription: description,
            }),
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
        })

        // If no error and we have user data, return it
        if (res.ok) {
            toast({
                title: '修改成功',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            window.location.href = `/request?request_id=${welcome.seekerId}`
            return;
        }
        // Return null if user data could not be retrieved
        else {
            const response = await res.json()
            toast({
                title: '修改失败',
                description: `state: ${res.status}, message: ${response.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    }

    const handleDelete = async () => {
        const res = await fetch(`${config.serverIp}/offers/${welcome.offerId}`, {
            method: 'DELETE',
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
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

    return (
        <>
            <Card align='center' w='750px' h='auto' bg="transparent" boxShadow="none">
                <CardBody>
                    <Flex w='700px' >
                        <Image borderRadius="full" boxSize="40px" src="/userIcon.png" alt="User Icon" />
                        <Box w="4" />
                        <Stack>
                            <Heading fontSize='md' >{welcome.username}</Heading>
                            <Textarea
                                w='620px'
                                h='400px'
                                value={description}
                                onChange={(e) => setDescription(e.target.value)}
                                placeholder={welcome.offerDescription}
                            />
                        </Stack>
                    </Flex>
                    <Box h='2' />
                    <Flex w='750px' justify='flex-end'>
                        <Button colorScheme='facebook' onClick={handleModify}>修改回复</Button>
                        <Box w='4' />
                        <Button colorScheme='red' onClick={onOpen}>删除回复</Button>
                        <AlertDialog
                            isOpen={isOpen}
                            leastDestructiveRef={cancelRef}
                            onClose={onClose}
                        >
                            <AlertDialogOverlay>
                                <AlertDialogContent>
                                    <AlertDialogHeader fontSize='lg' fontWeight='bold'>
                                        删除回复
                                    </AlertDialogHeader>

                                    <AlertDialogBody>
                                        你确定要删除这条回复吗？此操作不可撤销，你将无法在对此请求进行回复。
                                    </AlertDialogBody>

                                    <AlertDialogFooter>
                                        <Button ref={cancelRef} onClick={onClose}>
                                            取消
                                        </Button>
                                        <Button colorScheme='red' onClick={() => { handleDelete(); onClose(); }} ml={3}>
                                            删除
                                        </Button>
                                    </AlertDialogFooter>
                                </AlertDialogContent>
                            </AlertDialogOverlay>
                        </AlertDialog>
                    </Flex>

                </CardBody>
            </Card>
        </>
    );
}