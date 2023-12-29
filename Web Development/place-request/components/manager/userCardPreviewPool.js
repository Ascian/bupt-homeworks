'use client'

import {
    Box,
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
    Flex,
    Stack,

} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useSession } from 'next-auth/react';
import { useSearchParams } from 'next/navigation';

import Pagination from "../shared/pagination";
import UserCardPreview from './userCardPreview';
import config from '@/app/config';
import Filter from '../shared/filter';
import UsersFilter from '@/app/manager/users/usersFilter'
import { fetchData } from 'next-auth/client/_utils';

export default function UserCardPreviewPool() {
    const { data: session, status } = useSession();
    const [maxPage, setMaxPage] = useState(0);
    const [users, setUsers] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [isFilterSubmit, setIsFilterSubmit] = useState(false);

    const searchParams = useSearchParams()
    const userId = searchParams.get('user_id');
    const page = searchParams.get('page') || 1;


    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
            if (searchParams.has('user_id')) {
                fetch(`${config.serverIp}/admin/users/${userId}`, {
                    method: 'GET',
                    headers: {
                        "Authorization": `Bearer ${session.accessToken}`
                    }
                }).then((res) => res.json())
                    .then((response) => {
                        if (response?.id != undefined || response?.id != null) {
                            setUsers([response]);
                            setMaxPage(1);
                            setIsLoading(false);
                            isFetched = true;
                        }
                    })
            }
            else {
                let isUserListOk = false;
                let isCountOk = false;

                Promise.all([
                    fetch(`${config.serverIp}/admin/users?page=${page}&page_size=10`, {
                        method: 'GET',
                        headers: {
                            "Authorization": `Bearer ${session.accessToken}`
                        }

                    }).then((res) => res.json())
                        .then((userList) => {
                            if (userList != undefined || userList != null) {
                                setUsers(userList);
                                isUserListOk = true;
                            }
                        }),
                    fetch(`${config.serverIp}/admin/users/count`, {
                        method: 'GET',
                        headers: {
                            'Authorization': `Bearer ${session.accessToken}`
                        }
                    }).then((res) => res.json())
                        .then((count) => {
                            if (count != undefined || count != null) {
                                setMaxPage(Math.ceil(count / 10));
                                isCountOk = true;
                            }
                        })
                ]).then(() => {
                    if (isUserListOk && isCountOk) {
                        setIsLoading(false);
                        isFetched = true;
                    }
                })
            }
        }
    }, [status, searchParams])

    if (isLoading) {
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

    return (
        <>
            <Flex
                position="fixed"
                top="30%"
                left="10%"
                transform="translate(-30%, -15%)"
                w='auto'
                h='60%'
                zIndex="10"
            >
                <Filter setIsSubmit={setIsFilterSubmit}>
                    <UsersFilter isSubmit={isFilterSubmit} setIsSubmit={setIsFilterSubmit} />
                </Filter>
            </Flex>
            <Stack justify="center" align="center" w='full'>
                {maxPage == 0 ? (
                    <Card>
                        <Alert status="warning">
                            <AlertIcon />
                            <AlertTitle >此类用户不存在！</AlertTitle>
                        </Alert>
                    </Card>
                ) : (
                    <>
                        {
                            users.map((user) => (
                                <>
                                    <UserCardPreview user={user} />
                                    <Box h='2' />
                                </>
                            ))
                        }
                    </>
                )}
            </Stack>
            < Box h='10' />
            <Pagination maxPage={maxPage} />
        </>
    )
}