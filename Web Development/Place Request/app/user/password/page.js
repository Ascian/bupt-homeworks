'use client'

import UserIcon from '@/components/shared/userIcon';
import {
    Button,
    Card,
    CardBody,
    CardFooter,
    CardHeader,
    Stack,
    Grid,
    GridItem,
    Heading,
    Input,
    Text,
    Alert,
    AlertIcon,
    AlertTitle,

} from '@chakra-ui/react';
import { useToast } from '@chakra-ui/toast';
import { useSession } from 'next-auth/react';
import { useState } from 'react';
import config from '@/app/config'

export default function Page() {
    const [show, setShow] = useState(false);
    const { data: session } = useSession();

    const [originPassword, setOriginalPassword] = useState('');
    const [password, setPassword] = useState('');
    const [confirmPassword, setConfirmPassword] = useState('');

    const [isOriginalPasswordInvalid, setIsOriginalPasswordInvalid] = useState(false);
    const [isPasswordInvalid, setIsPasswordInvalid] = useState(false);
    const [isConfirmPasswordInvalid, setIsConfirmPasswordInvalid] = useState(false);

    const toast = useToast()
    const handleSubmit = async () => {
        if (originPassword === '' || originPassword.length > 20) {
            toast({
                title: '原始密码不合法',
                description: "原始密码不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsOriginalPasswordInvalid(true);
            return;
        }

        // password must contain 2 numbers, and is not allowed to only contain uppercase or lowercase letters
        let digitCount = 0;
        for (let i = 0; i < password.length; i++) {
            if (password[i] >= '0' && password[i] <= '9') {
                digitCount++;
            }
        }
        if (digitCount < 2) {
            toast({
                title: '密码不合法',
                description: "密码必须包含至少2个数字",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }
        if (password.toUpperCase() === password || password.toLowerCase() === password) {
            toast({
                title: '密码不合法',
                description: "密码不能只包含大写或小写字母",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }

        if (password < 6 || password.length > 20) {
            toast({
                title: '密码不合法',
                description: "密码长度不能小于6且不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }
        if (password !== confirmPassword) {
            toast({
                title: '密码不一致',
                description: "两次输入密码一致",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsConfirmPasswordInvalid(true);
            return;
        }

        const res = await fetch(`${config.serverIp}/users/me`, {
            method: 'PATCH',
            body: JSON.stringify({
                originalPassword: originPassword,
                password: password,
            }),
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
        })
        const user = await res.json()

        // If no error and we have user data, return it
        if (res.ok && user) {
            toast({
                title: '修改成功',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            window.location.href = '/user'
            return;
        }
        // Return null if user data could not be retrieved
        else {
            toast({
                title: '修改失败',
                description: `${user.errorMessage}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    };

    return (
        <>
            {session ? (
                <Card align='center'>
                    <CardHeader>
                        <Stack align='center'>
                            <UserIcon boxSize='100px' />
                            <Heading size='lg' >{session.user.username}</Heading>
                        </Stack>
                    </CardHeader >

                    <CardBody>
                        <Grid templateRows='repeat(2, 1fr)' templateColumns='repeat(2, 1fr)' gap={1}>
                            <GridItem rowSpan={1} colSpan={1} p='4'>
                                <Text>原始密码</Text>
                                <Input
                                    value={originPassword}
                                    type={show ? 'text' : 'password'}
                                    onChange={(e) => setOriginalPassword(e.target.value)}
                                    errorBorderColor='crimson'
                                    isInvalid={isOriginalPasswordInvalid}
                                />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={1} p='4'>
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={1} p='4'>
                                <Text>密码</Text>
                                <Input
                                    value={password}
                                    type={show ? 'text' : 'password'}
                                    onChange={(e) => setPassword(e.target.value)}
                                    errorBorderColor='crimson'
                                    isInvalid={isPasswordInvalid}
                                />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={1} p='4'>
                                <Text>确认密码</Text>
                                <Input
                                    value={confirmPassword}
                                    type={show ? 'text' : 'password'}
                                    onChange={(e) => setConfirmPassword(e.target.value)}
                                    errorBorderColor='crimson'
                                    isInvalid={isConfirmPasswordInvalid}
                                />
                            </GridItem>
                        </Grid>
                    </CardBody>
                    <CardFooter>
                        <Button colorScheme='telegram' size='lg' onClick={() => handleSubmit()}>确认修改</Button>
                    </CardFooter>
                </Card >
            ) : (
                <Card align='center'>
                    <Alert status='error'>
                        <AlertIcon />
                        <AlertTitle>请先登录!</AlertTitle>
                    </Alert>
                </Card >
            )
            }
        </>
    );
}