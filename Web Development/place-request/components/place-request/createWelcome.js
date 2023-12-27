'use client'

import {
    Box,
    Card,
    CardBody,
    Flex,
    Text,
    Textarea,
    Button,
} from "@chakra-ui/react";
import { useToast } from '@chakra-ui/toast';
import { useState } from 'react';
import { useSession } from 'next-auth/react';
import config from '@/app/config'

export default async function CreateWelcome({ requestId }) {
    const [description, setDescription] = useState('');
    const { data: session } = useSession();
    const toast = useToast()

    const handleClick = async () => {
        if (description.length > 10000) {
            toast({
                title: '描述不合法',
                description: "描述长度不能超过10000",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsDescriptionInvalid(true);
            return;
        }

        const res = await fetch(`${config.serverIp}/offers`, {
            method: 'POST',
            body: JSON.stringify({
                seekerId: requestId,
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
                title: '提交成功',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
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

    return (
        <>
            <Card align='center' w='800px' h='500px'>
                <CardBody>
                    <Flex align='center' w='700px' >
                        <Text fontSize='20px' >欢迎来：</Text>
                        <Textarea
                            w='600px'
                            h='400px'
                            placeholder="请输入详细描述"
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                        />
                    </Flex>
                    <Box h='4' />
                    <Flex align='center' justify='flex-end' w='670px' >
                        <Button onClick={handleClick} colorScheme='telegram' >提交</Button>
                    </Flex>
                </CardBody>
            </Card>
        </>
    );
}