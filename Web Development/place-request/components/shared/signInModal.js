'use client'

import { useState, useCallback, useMemo } from 'react';
import {
    Modal,
    ModalOverlay,
    ModalContent,
    ModalHeader,
    ModalCloseButton,
    ModalBody,
    ModalFooter,
    FormControl,
    FormLabel,
    Input,
    Button,
    Link
} from '@chakra-ui/react';
import { useToast } from "@chakra-ui/react"
import { signIn } from 'next-auth/react';

export function SignInModal({
    showSignInModal,
    setShowSignInModal }) {
    const toast = useToast()

    // handle logic for showing password
    const [show, setShow] = useState(false);

    // handle logic for signing in
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const handleSignIn = async () => {
        if (username === "" || username.length > 20) {
            toast({
                title: "用户名错误",
                description: "用户名不能为空且长度不能超过20",
                status: "error",
                duration: 9000,
                isClosable: true,
            })
            return null
        }
        if (password === "" || password.length > 20) {
            toast({
                title: "密码错误",
                description: "密码不能为空且长度不能超过20",
                status: "error",
                duration: 9000,
                isClosable: true,
            })
            return null
        }

        const result = await signIn('credentials', {
            redirect: false,
            username: username,
            password: password,
        });
        if (result?.error) {
            toast({
                title: "登录失败",
                description: '用户名或密码错误',
                status: "error",
                duration: 9000,
                isClosable: true,
            })
        } else {
            setShowSignInModal(false);
        }
    };

    return (
        <Modal
            isOpen={showSignInModal}
            onClose={setShowSignInModal}
        >
            <ModalOverlay
                bg="whiteAlpha.500"
                backdropFilter="blur(10px)" />
            <ModalContent>
                <ModalHeader>登录</ModalHeader>
                <ModalCloseButton />
                <ModalBody pb={6}>
                    <FormControl>
                        <FormLabel>用户名</FormLabel>
                        <Input
                            onChange={(e) => setUsername(e.target.value)}
                            placeholder='请输入用户名' />
                    </FormControl>

                    <FormControl mt={4}>
                        <FormLabel>密码</FormLabel>
                        <Input
                            onChange={(e) => setPassword(e.target.value)}
                            type={show ? 'text' : 'password'}
                            placeholder='请输入密码' />
                    </FormControl>
                </ModalBody>

                <ModalFooter>
                    <Link href="/register">
                        <Button mr={3}>
                            注册
                        </Button>
                    </Link>
                    <Button colorScheme='telegram' mr={3} onClick={handleSignIn}>
                        登录
                    </Button>
                </ModalFooter>
            </ModalContent>
        </Modal>
    );
}

export default function useSignInModal() {
    const [showSignInModal, setShowSignInModal] = useState(false);

    const SignInModalCallback = useCallback(() => {
        return (
            <SignInModal
                showSignInModal={showSignInModal}
                setShowSignInModal={setShowSignInModal}
            />
        );
    }, [showSignInModal, setShowSignInModal]);

    return useMemo(
        () => ({ SignInModal: SignInModalCallback, setShowSignInModal }),
        [SignInModalCallback, setShowSignInModal],
    );
}